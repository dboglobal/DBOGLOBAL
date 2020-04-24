CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3003;
	title = 300302;

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
				itype = 5;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 300309;
				ctype = 1;
				idx = 1502202;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99158;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 3003;
				cnt0 = 10;
				ectype = -1;
				etype = 0;
				qtidx = 300315;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 300308;
				m1fx = "-51.360001";
				m2fz = "-833.479980";
				m1fy = "0.000000";
				sort = 300305;
				m2widx = 6;
				grade = -1;
				m1pat = 2;
				m1widx = 6;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 300301;
				goal = 300304;
				m0fz = "-814.049988";
				m0widx = 6;
				m1ttip = 300315;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = 99158;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-634.380005";
				m2fx = "73.190002";
				m2ttip = 300315;
				m0fx = "10.850000";
				m0ttip = 300315;
				title = 300302;
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
				iidx0 = 99158;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 300314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 300301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1502202;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 300307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "1502202;";
			}
			CDboTSCheckClrQst
			{
				and = "3002;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 300308;
			gtype = 2;
			oklnk = 2;
			area = 300301;
			goal = 300304;
			sort = 300305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 300302;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

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
				iidx0 = 99158;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

