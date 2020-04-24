CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2616;
	title = 261602;

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
				conv = 261609;
				ctype = 1;
				idx = 6311102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 261608;
				gtype = 1;
				area = 261601;
				goal = 261604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 261605;
				stype = 2;
				taid = 1;
				title = 261602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 261607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "6311102;";
			}
			CDboTSCheckClrQst
			{
				and = "2615;";
				flink = 1;
				flinknextqid = "2617;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 261608;
			gtype = 1;
			oklnk = 2;
			area = 261601;
			goal = 261604;
			sort = 261605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 261602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 261614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 261601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1651109;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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

