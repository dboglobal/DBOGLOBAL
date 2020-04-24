CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2999;
	title = 299902;

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
				conv = 299909;
				ctype = 1;
				idx = 4511314;
				taid = 2;
			}
			CDboTSActETimerS
			{
				sort = 0;
				taid = 3;
				tgid = 1;
				time = 300000;
			}
			CDboTSActItem
			{
				iidx0 = 99160;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 299908;
				m1fx = "722.049988";
				m1fy = "0.000000";
				sort = 299905;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-866.969971";
				grade = 132203;
				m0fx = "821.909973";
				m0ttip = 299915;
				m1pat = 2;
				m1widx = 6;
				rwd = 100;
				taid = 1;
				title = 299902;
				gtype = 2;
				area = 299901;
				goal = 299904;
				m0fz = "-833.309998";
				m0widx = 6;
				m1ttip = 299915;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 5;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 299901;
				cnt0 = 6;
				ectype = -1;
				etype = 0;
				qtidx = 299915;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 299914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 299901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3171203;";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActETimerE
			{
				taeid = 3;
				taid = 1;
				tceid = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 299907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "4511314;";
			}
			CDboTSCheckClrQst
			{
				and = "2998;";
				flink = 1;
				flinknextqid = "3000;";
				not = 0;
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
				taid = 2;
				esctype = 0;
			}
			CDboTSActItem
			{
				iidx0 = 99160;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 299908;
			gtype = 2;
			oklnk = 2;
			area = 299901;
			goal = 299904;
			sort = 299905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 299902;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
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

			CDboTSActItem
			{
				iidx0 = 99160;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 299907;
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
	}
	CNtlTSGroup
	{
		gid = 1;

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

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 299907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

