CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 660;
	title = 66002;

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
				conv = 66009;
				ctype = 1;
				idx = 4371402;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 66008;
				gtype = 1;
				area = 66001;
				goal = 66004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 66005;
				stype = 1;
				taid = 1;
				title = 66002;
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
			cont = 66008;
			gtype = 1;
			oklnk = 2;
			area = 66001;
			goal = 66004;
			sort = 66005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 66002;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 66014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 66001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3032102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 66007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "4371402;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "661;";
				not = 0;
			}
		}
	}
}

