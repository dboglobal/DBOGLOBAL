CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3770;
	title = 377002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "1654115;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3771;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 377009;
				ctype = 1;
				idx = 1654115;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 377008;
				m1fx = "-3945.114990";
				m1fy = "0.000000";
				sort = 377005;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "-6425.038086";
				grade = 132203;
				m0fx = "-3943.780029";
				m0ttip = 377015;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				title = 377002;
				gtype = 1;
				area = 377001;
				goal = 377004;
				m0fz = "-6415.040039";
				m0widx = 15;
				m1ttip = 377016;
				scitem = -1;
				stype = 2;
			}
			CDboTSActSToCEvt
			{
				widx1 = 15;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 377020;
				esctype = 0;
				iidx1 = -1;
				it1 = -1;
				otype0 = 1;
				dt1 = 377021;
				etype = 0;
				it0 = -1;
				taid = 3;
				tidx1 = 3148209;
				widx0 = 15;
				apptype = 0;
				eitype = 6;
				tidx0 = 1551301;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 377014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 377001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1654115;";
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 377008;
			gtype = 1;
			oklnk = 2;
			area = 377001;
			goal = 377004;
			sort = 377005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 377002;
		}
	}
}

