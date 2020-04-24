CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2965;
	title = 296502;

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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 296507;
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
				npcidx = "4751210;";
			}
			CDboTSCheckClrQst
			{
				and = "2964;";
				flink = 0;
				flinknextqid = "2966;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 296514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 296501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751210;";
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
			cont = 296508;
			gtype = 2;
			oklnk = 2;
			area = 296501;
			goal = 296504;
			sort = 296505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 296502;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 296509;
				ctype = 1;
				idx = 4751210;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 15891400;
				cnt0 = 6;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 15791100;
				taid = 3;
				esctype = 0;
				idx2 = 15892100;
			}
			CDboTSActRegQInfo
			{
				cont = 296508;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "4305.609863";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "4087.870117";
				m4ttip = 296517;
				area = 296501;
				goal = 296504;
				m0fz = "-4695.319824";
				m0widx = 1;
				m1ttip = 296516;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-4231.040039";
				m3ttip = 296517;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4233.259766";
				m2fx = "4109.359863";
				m2ttip = 296516;
				m4widx = 1;
				m0fx = "4594.669922";
				m0ttip = 296515;
				m3pat = 2;
				m4pat = 2;
				title = 296502;
				m1fx = "4383.879883";
				m2fz = "-4335.549805";
				sort = 296505;
				m4fz = "-4329.770020";
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
	}
}

