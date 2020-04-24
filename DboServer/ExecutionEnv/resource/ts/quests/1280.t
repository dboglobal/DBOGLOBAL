CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1280;
	title = 128002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 128009;
				ctype = 1;
				idx = 1551107;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 175;
				cnt0 = 3;
				ectype = -1;
				etype = 0;
				qtidx = 128030;
				taid = 3;
				esctype = 0;
			}
			CDboTSActQItem
			{
				icnt0 = 3;
				iprob0 = "1.000000";
				iidx0 = 808;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 128008;
				m1fx = "1807.000000";
				m2fz = "3544.000000";
				m1fy = "0.000000";
				sort = 128005;
				m2widx = 1;
				grade = 132203;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 128001;
				goal = 128004;
				m0fz = "4039.000000";
				m0widx = 1;
				m1ttip = 128015;
				m2fy = "0.000000";
				m2pat = 0;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "4198.000000";
				m2fx = "1993.000000";
				m2ttip = 128015;
				m0fx = "2200.000000";
				m0ttip = 128015;
				title = 128002;
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
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 128014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 128001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1551107;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 128007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSCheckClrQst
			{
				and = "1279;";
				flink = 1;
				flinknextqid = "1281;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1551107;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 128008;
			gtype = 2;
			oklnk = 2;
			area = 128001;
			goal = 128004;
			sort = 128005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 128002;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 253;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 808;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 128007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

