CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 3760;
	title = 376002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 376007;
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
				npcidx = "8101101;";
			}
			CDboTSCheckClrQst
			{
				and = "3759;";
				flink = 0;
				flinknextqid = "3761;";
				not = 0;
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
				eitype = 0;
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
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
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
				conv = 376009;
				ctype = 1;
				idx = 8101101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 19843200;
				cnt0 = 50;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 376008;
				m1fx = "-3964.340088";
				m2fz = "-5649.029785";
				m1fy = "0.000000";
				sort = 376005;
				m1fz = "-5506.870117";
				m2fx = "-4079.729980";
				m2ttip = 376015;
				m2widx = 15;
				grade = 132203;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				title = 376002;
				gtype = 2;
				area = 376001;
				goal = 376004;
				m1ttip = 376015;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 376014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 376001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8101101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 376008;
			gtype = 2;
			oklnk = 2;
			area = 376001;
			goal = 376004;
			sort = 376005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 376002;
		}
	}
}

