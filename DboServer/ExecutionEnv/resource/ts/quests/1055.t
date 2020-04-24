CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1055;
	title = 105502;

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
				conv = 105509;
				ctype = 1;
				idx = 4071104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 105508;
				gtype = 1;
				area = 105501;
				goal = 105504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 105505;
				stype = 2;
				taid = 1;
				title = 105502;
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
			cont = 105508;
			gtype = 1;
			oklnk = 2;
			area = 105501;
			goal = 105504;
			sort = 105505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 105502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 105514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 105501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071113;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 105507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1056;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4071104;";
			}
		}
	}
}

