CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2944;
	title = 294402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1531;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActNPCConv
			{
				conv = 294409;
				ctype = 1;
				idx = 4511414;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 294408;
				m1fx = "3364.780029";
				m2fz = "-4151.419922";
				m1fy = "0.000000";
				sort = 294405;
				m2widx = 1;
				grade = 132203;
				m0fx = "3431.620117";
				m0ttip = 294415;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 294402;
				gtype = 2;
				area = 294401;
				goal = 294404;
				m0fz = "-3978.909912";
				m0widx = 1;
				m1ttip = 294415;
				m2fy = "0.000000";
				m2pat = 2;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4225.080078";
				m2fx = "3592.879883";
				m2ttip = 294415;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 294414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 294401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511414;";
			}
			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 294408;
			gtype = 2;
			oklnk = 2;
			area = 294401;
			goal = 294404;
			sort = 294405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 294402;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
			stdiag = 294407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSCheckClrQst
			{
				and = "2936;";
				flink = 1;
				flinknextqid = "2945;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4511414;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1531;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 294407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

