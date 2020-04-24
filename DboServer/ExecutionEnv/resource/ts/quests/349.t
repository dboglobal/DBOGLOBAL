CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 1;
	rq = 0;
	tid = 349;
	title = 34902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "8;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 34920;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActETimerE
			{
				taeid = 2;
				taid = 1;
				tceid = 7;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 34907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 8;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSClickObject
			{
				objidx = "420;";
				widx = 1;
			}
			CDboTSCheckClrQst
			{
				and = "348;";
				flink = 1;
				not = 0;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 34908;
				area = 34901;
				goal = 34904;
				m0fz = "-3241.000000";
				m0widx = 1;
				scitem = -1;
				sort = 34905;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "4541.000000";
				m0ttip = 34915;
				rwd = 100;
				taid = 1;
				title = 34902;
				gtype = 3;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 34914;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 34901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511602;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 34908;
			gtype = 3;
			oklnk = 2;
			area = 34901;
			goal = 34904;
			sort = 34905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 34902;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 101;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 349;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 7;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 349;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 4;
			prelnk = "3;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 2;
				tgid = 1;
				time = 300000;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 4031102;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "7;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;2;";
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
			prelnk = "0;252;1;";
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
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

