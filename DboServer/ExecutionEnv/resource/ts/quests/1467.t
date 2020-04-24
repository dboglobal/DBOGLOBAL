CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1467;
	title = 146702;

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
				conv = 146709;
				ctype = 1;
				idx = 2803101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 146708;
				gtype = 2;
				area = 146701;
				goal = 146704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 146705;
				stype = 1;
				taid = 1;
				title = 146702;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 146714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 146701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2802101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 146708;
			gtype = 2;
			oklnk = 2;
			area = 146701;
			goal = 146704;
			sort = 146705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 146702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 146707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSCheckClrQst
			{
				and = "1450;";
				flink = 1;
				flinknextqid = "1475;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2803101;";
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
	}
}

