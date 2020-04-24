CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 391;
	title = 39102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 39107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "7571109;";
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99170;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "4;5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSItemUpgrade
			{
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 5;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99170;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 3;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 99170;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 39108;
			gtype = 1;
			oklnk = 2;
			area = 39101;
			goal = 39104;
			sort = 39105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 39102;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 39109;
				ctype = 1;
				idx = 7571109;
				taid = 2;
			}
			CDboTSActOpenWindow
			{
				show = 1;
				taid = 3;
				tblidx = 20005;
				wtype = 5;
			}
			CDboTSActRegQInfo
			{
				cont = 39108;
				gtype = 1;
				area = 39101;
				goal = 39104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 39105;
				stype = 64;
				taid = 1;
				title = 39102;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 39114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 39101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7571109;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 39107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99170;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
}

