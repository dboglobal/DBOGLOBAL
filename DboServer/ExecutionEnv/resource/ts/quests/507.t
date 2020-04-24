CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 2;
	rq = 0;
	tid = 507;
	title = 50702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 656;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;";
			nolnk = 254;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 656;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 507;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 10;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSRcvSvrEvt
			{
				id = 80;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 100;
			prelnk = "10;";

			CDboTSActMiniNarration
			{
				taid = 1;
				tidx = 5073;
			}
			CDboTSActETimerE
			{
				taeid = 1;
				taid = 3;
				tceid = 4;
			}
			CDboTSActETimerE
			{
				taeid = 2;
				taid = 4;
				tceid = 4;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 656;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 50707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "4151202;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckClrQst
			{
				and = "497;";
				flink = 1;
				flinknextqid = "508;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 50714;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 50701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "11;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151202;";
			}
			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 656;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 10;
			prelnk = "3;";

			CDboTSActETimerS
			{
				sort = 1;
				taid = 2;
				tgid = 12;
				time = 2000;
			}
			CDboTSActETimerS
			{
				sort = 1;
				taid = 1;
				tgid = 11;
				time = 2000;
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
				cont = 50708;
				gtype = 3;
				area = 50701;
				goal = 50704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 50705;
				stype = 2;
				taid = 1;
				title = 50702;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 50708;
			gtype = 3;
			oklnk = 2;
			area = 50701;
			goal = 50704;
			sort = 50705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 50702;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;5;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;4;2;";
		}
	}
	CNtlTSGroup
	{
		gid = 11;

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
			stdiag = 50707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckDistWithNPC
			{
				dist = "30.000000";
				idxlist = "4151207;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 40507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 656;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 656;
				taid = 1;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 12;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 50707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckNPCDead
			{
				idxlist = "4151207;";
			}
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

