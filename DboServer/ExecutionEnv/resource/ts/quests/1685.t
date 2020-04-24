CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1685;
	title = 168502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 168509;
				ctype = 1;
				idx = 4071117;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 168508;
				gtype = 1;
				area = 168501;
				goal = 168504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 168505;
				stype = 2;
				taid = 1;
				title = 168502;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 168507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "4071117;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1705;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 168508;
			gtype = 1;
			oklnk = 2;
			area = 168501;
			goal = 168504;
			sort = 168505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 168502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 168514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 168501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653110;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

