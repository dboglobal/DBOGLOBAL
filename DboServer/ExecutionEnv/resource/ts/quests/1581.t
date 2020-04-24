CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 6;
	rq = 0;
	tid = 1581;
	title = 158102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 158107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "3241102;";
			}
			CDboTSCheckClrQst
			{
				and = "1577;";
				flink = 1;
				flinknextqid = "1582;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActETimerE
			{
				taeid = 5;
				taid = 1;
				tceid = 2;
			}
			CDboTSActSendSvrEvt
			{
				id = 185;
				tblidx = 1581;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 0;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 158109;
				ctype = 1;
				idx = 3241102;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 1581;
				start = 1;
				taid = 4;
				uof = 0;
			}
			CDboTSActETimerS
			{
				sort = 0;
				taid = 5;
				tgid = 1;
				time = 600000;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 5653102;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5652103;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 158108;
				m1fx = "-1250.280029";
				m1fy = "0.000000";
				sort = 158105;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "893.280029";
				grade = 132203;
				m0fx = "-1115.680054";
				m0ttip = 158115;
				m1pat = 1;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 158102;
				gtype = 3;
				area = 158101;
				goal = 158104;
				m0fz = "1121.410034";
				m0widx = 1;
				m1ttip = 158116;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 158114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 158101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3241102;";
			}
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 158108;
			gtype = 3;
			oklnk = 2;
			area = 158101;
			goal = 158104;
			sort = 158105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 158102;
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 158107;
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

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
	}
}

