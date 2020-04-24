CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2460;
	title = 246002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2460;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 101;

			CDboTSClickNPC
			{
				npcidx = "4072301;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "101;2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 246007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "4141116;";
			}
			CDboTSCheckClrQst
			{
				and = "2459;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "101;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 562;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 246014;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 246001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072301;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 246009;
				ctype = 1;
				idx = 4141116;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 246008;
				gtype = 3;
				area = 246001;
				goal = 246004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 246005;
				stype = 2;
				taid = 1;
				title = 246002;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 252;
			nextlnk = 3;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 2460;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 246008;
			gtype = 3;
			oklnk = 2;
			area = 246001;
			goal = 246004;
			sort = 246005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 246002;
		}
	}
}

