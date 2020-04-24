CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3011;
	title = 301102;

	CNtlTSGroup
	{
		gid = 11;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckNPCDead
			{
				idxlist = "4751415;";
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActETimerE
			{
				taeid = 2;
				taid = 2;
				tceid = 4;
			}
			CDboTSActSendSvrEvt
			{
				id = 780;
				tblidx = 3011;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 1;
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
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 3011;
				start = 0;
				taid = 3;
				uof = 0;
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
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1571;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActETimerS
			{
				sort = 1;
				taid = 2;
				tgid = 11;
				time = 2000;
			}
			CDboTSActWorldPlayScript
			{
				sid = 3011;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSColRgn
			{
				widx = 6;
				cct = 1;
				crt = 0;
				r = "40.000000";
				x = "910.909973";
				z = "76.629997";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 301107;
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
				npcidx = "4751414;";
			}
			CDboTSCheckClrQst
			{
				and = "2993;";
				flink = 0;
				flinknextqid = "3012;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 0;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 301101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751414;";
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
				conv = 301109;
				ctype = 1;
				idx = 4751414;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1571;
				cnt0 = 1;
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
				cont = 301108;
				area = 301101;
				goal = 301104;
				m0fz = "76.629997";
				m0widx = 6;
				scitem = -1;
				sort = 301105;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "910.909973";
				m0ttip = 301115;
				rwd = 100;
				taid = 1;
				title = 301102;
				gtype = 3;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 301108;
			gtype = 3;
			oklnk = 2;
			area = 301101;
			goal = 301104;
			sort = 301105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 301102;
		}
	}
}

