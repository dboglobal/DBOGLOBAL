CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1437;
	title = 143702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "8032104;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1035;
			}
			CDboTSClickNPC
			{
				npcidx = "8032104;";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 1437;
				start = 0;
				taid = 4;
				uof = 0;
			}
			CDboTSActETimerE
			{
				taeid = 3;
				taid = 3;
				tceid = 2;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1035;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
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

			CDboTSActETimerS
			{
				sort = 0;
				taid = 3;
				tgid = 1;
				time = 600000;
			}
			CDboTSActWorldPlayScript
			{
				sid = 1437;
				start = 1;
				taid = 4;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 143708;
				gtype = 1;
				area = 143701;
				goal = 143704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 143705;
				stype = 1;
				taid = 1;
				title = 143702;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92031;
				cnt1 = 0;
				eitype = 2;
				idx0 = 1035;
				cnt0 = 1;
				ectype = 0;
				esctype = 0;
				idx2 = -1;
				cnt2 = 0;
				etype = 0;
				idx1 = -1;
				taid = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 143714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 143701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;4;";
			usetbl = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 143707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 46;
			}
			CDboTSClickObject
			{
				objidx = "693;";
				widx = 1;
			}
			CDboTSCheckClrQst
			{
				and = "1436;";
				flink = 1;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 143708;
			gtype = 1;
			oklnk = 2;
			area = 143701;
			goal = 143704;
			sort = 143705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 143702;
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
				iidx0 = 1035;
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
			stdiag = 143707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 1;

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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
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

